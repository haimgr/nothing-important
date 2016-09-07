package game.entities

import scala.collection.mutable
import scala.collection.mutable.ArrayBuffer

/**
  * Created by haim7 on 07 ספטמבר 2016.
  */
class Ordinal[T <: Ordinal[T]](creator: OrdinalCreator[T]) {

  private val self = this.asInstanceOf[T]

  val id: Int = creator.add(self)

  def +(that: T): Set[T] = Set[T](self, that)

  override def toString: String = getClass.getSimpleName + "(" + id + ")"
}

trait OrdinalCreator[T <: Ordinal[T]] {
  private val _values = new ArrayBuffer[T]
  def add(ordinal: T): Int = {
    val ret = _values.size
    _values += ordinal
    ret
  }
  def values: Seq[T] = {val ret = _values.toList; _values == null; ret}

}